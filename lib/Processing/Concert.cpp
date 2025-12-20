#include "Concert.hpp"

#include <cassert>
#include <list>

#include "IPatch.hpp"
#include "IProcessingBlockFactory.hpp"
#include "Json11Helper.hpp"

#define LOGGING_COMPONENT "Concert"

uint8_t Concert::lastProgramChange;
uint8_t Concert::lastProgramChangeXorCheck;

Concert::Concert(MidiInput& midiInput, IProcessingBlockFactory& processingBlockFactory)
    : midiInput(midiInput), processingBlockFactory(processingBlockFactory)
{
    midiInput.subscribe(*this);
}

Concert::~Concert()
{
    midiInput.unsubscribe(*this);

    for (auto patch : patches)
    {
        delete patch;
    }
}

size_t Concert::size() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return patches.size();
}

Concert::PatchPosition Concert::addPatch()
{
    std::lock_guard<std::mutex> lock(mutex);

    IPatch* patch = processingBlockFactory.createPatch();
    assert(patch != nullptr);

    return addPatchInternal(patch);
}

Concert::PatchPosition Concert::addPatch(IPatch* patch)
{
    std::lock_guard<std::mutex> lock(mutex);

    return addPatchInternal(patch);
}

Concert::PatchPosition Concert::addPatchInternal(IPatch* patch)
{
    patches.push_back(patch);

    if (patches.size() == 1)
    {
        // First patch. Activate it.
        patch->activate();
        activePatchPosition = 0;
    }

    return patches.size() - 1;
}

IPatch* Concert::getPatch(PatchPosition position) const
{
    std::lock_guard<std::mutex> lock(mutex);

    if (position >= patches.size())
    {
        return nullptr;
    }

    return patches.at(position);
}

bool Concert::removePatch(PatchPosition position)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (position >= patches.size())
    {
        return false;
    }

    patches.erase(patches.begin() + position);
    return true;
}

void Concert::reactivateLastActivePatch()
{
    if ((lastProgramChange ^ lastProgramChangeXorMask) == lastProgramChangeXorCheck)
    {
        LOG_INFO_PARAMS("retriggering last program change from before reboot (%u)",
                        lastProgramChange);
        onProgramChange(programChangeChannel, lastProgramChange);
    }
    else
        LOG_INFO("Last program change checksum validation failed, skipping patch reactivation");
}

Json Concert::convertToJson() const
{
    std::lock_guard<std::mutex> lock(mutex);

    Json::object converted;
    converted[JsonConvertible::objectTypeKey] = getObjectType();
    converted[programChangeChannelJsonKey] = programChangeChannel;
    converted[currentBankJsonKey] = currentBank;
    converted[noteToLightMapJsonKey] = processing::convert(noteToLightMap);

    Json::array convertedPatches;
    for (const IPatch* patch : patches)
    {
        convertedPatches.push_back(patch->convertToJson());
    }
    converted[patchesJsonKey] = convertedPatches;

    return Json(converted);
}

void Concert::convertFromJson(const Json& converted)
{
    std::lock_guard<std::mutex> lock(mutex);

    Json11Helper helper(__PRETTY_FUNCTION__, converted);
    helper.getItemIfPresent(programChangeChannelJsonKey, programChangeChannel);
    helper.getItemIfPresent(currentBankJsonKey, currentBank);

    Json::object convertedNoteToLightMap;
    if (helper.getItemIfPresent(noteToLightMapJsonKey, convertedNoteToLightMap))
    {
        noteToLightMap = processing::convert(convertedNoteToLightMap);
        // Make sure all mapped lights fit into the strip
        createMinimumAmountOfLights();
    }

    for (IPatch* patch : patches)
    {
        delete patch;
    }
    patches.clear();

    Json::array convertedPatches;
    if (helper.getItemIfPresent(patchesJsonKey, convertedPatches))
    {
        for (const Json& convertedPatch : convertedPatches)
        {
            patches.push_back(processingBlockFactory.createPatch(convertedPatch));
        }
    }
}

processing::NoteToLightMap Concert::getNoteToLightMap() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return noteToLightMap;
}

void Concert::setNoteToLightMap(processing::NoteToLightMap noteToLightMap)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->noteToLightMap = noteToLightMap;

    // Make sure all mapped lights fit into the strip
    createMinimumAmountOfLights();
}

void Concert::createMinimumAmountOfLights()
{
    uint16_t highestLightIndex(0);
    for (const auto& pair : noteToLightMap)
    {
        if (pair.second > highestLightIndex)
        {
            highestLightIndex = pair.second;
        }
    }

    size_t minimumAmount(highestLightIndex + 1);
    if (strip.size() < minimumAmount)
        strip.resize(minimumAmount);
}

size_t Concert::getStripSize() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return strip.size();
}

void Concert::setStripSize(size_t size)
{
    std::lock_guard<std::mutex> lock(mutex);

    strip.resize(size);
}

uint8_t Concert::getProgramChangeChannel() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return programChangeChannel;
}

void Concert::setProgramChangeChannel(uint8_t programChangeChannel)
{
    std::lock_guard<std::mutex> lock(mutex);

    this->programChangeChannel = programChangeChannel;
}

uint16_t Concert::getCurrentBank() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return currentBank;
}

void Concert::setCurrentBank(uint16_t bank)
{
    std::lock_guard<std::mutex> lock(mutex);

    currentBank = bank;
}

void Concert::execute()
{
    scheduler.executeAll();

    std::lock_guard<std::mutex> lock(mutex);

    if (activePatchPosition != invalidPatchPosition)
    {
        patches.at(activePatchPosition)->execute(strip, noteToLightMap);

        for (auto observer : observers)
        {
            observer->onStripUpdate(strip);
        }
    }
}

void Concert::subscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(mutex);
    observers.push_back(&observer);
}

void Concert::unsubscribe(IObserver& observer)
{
    std::lock_guard<std::mutex> lock(mutex);
    observers.remove(&observer);
}

std::string Concert::getObjectType() const
{
    return typeName;
}

void Concert::onProgramChange(uint8_t channel, uint8_t program)
{
    auto taskFn = [=]()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (channel != programChangeChannel)
            return;

        lastProgramChange = program;
        lastProgramChangeXorCheck = program ^ lastProgramChangeXorMask;

        for (auto patchIt = patches.begin(); patchIt != patches.end(); ++patchIt)
        {
            IPatch* patch = *patchIt;
            if (patch->hasBankAndProgram())
            {
                if (patch->getBank() == currentBank)
                {
                    if (patch->getProgram() == program)
                    {
                        // Found a patch which matches the received program number and active
                        // bank.
                        if (activePatchPosition != invalidPatchPosition)
                        {
                            IPatch* activePatch(patches.at(activePatchPosition));
                            LOG_INFO_PARAMS("deactivating patch '%s'",
                                            activePatch->getName().c_str());
                            activePatch->deactivate();
                        }
                        LOG_INFO_PARAMS("activating patch '%s'", patch->getName().c_str());
                        patch->activate();
                        activePatchPosition = patchIt - patches.begin();
                    }
                }
            }
        }
    };
    scheduler.schedule(taskFn);
}

void Concert::onControlChange(uint8_t channel, MidiInterface::ControllerNumber controllerNumber,
                              uint8_t value)
{
    if ((controllerNumber != MidiInterface::bankSelectMsb) &&
        (controllerNumber != MidiInterface::bankSelectLsb))
    {
        return;
    }

    auto taskFn = [=]()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (channel != programChangeChannel)
        {
            return;
        }

        if (controllerNumber == MidiInterface::bankSelectMsb)
        {
            currentBank = (value << 7) | (currentBank & 0x7f);
        }
        else if (controllerNumber == MidiInterface::bankSelectLsb)
        {
            currentBank = (currentBank & 0x7f80) | value;
        }
    };
    scheduler.schedule(taskFn);
}
