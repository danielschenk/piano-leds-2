/**
 * @file
 *
 * MIT License
 *
 * @copyright (c) 2017 Daniel Schenk <danielschenk@users.noreply.github.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @brief Interface for processing blocks.
 */

#ifndef PROCESSING_IPROCESSINGBLOCK_H_
#define PROCESSING_IPROCESSINGBLOCK_H_

#include <vector>

#include "ProcessingTypes.h"
#include "IJsonConvertible.h"

/**
 * Interface for processing blocks.
 */
class IProcessingBlock
    : public IJsonConvertible
{
public:
    static constexpr const char* typeNameEqualRangeRgbSource  = "EqualRangeRgbSource";
    static constexpr const char* typeNameNoteRgbSource        = "NoteRgbSource";
    static constexpr const char* typeNameProcessingChain      = "ProcessingChain";

    /**
     * Destructor.
     */
    virtual ~IProcessingBlock() = default;

    /**
     * Activate this block.
     *
     * @post The block responds to events.
     */
    virtual void activate() = 0;

    /**
     * Deactivate this block.
     *
     * @post The block ignores events.
     * @post The block is in a clean state (as if no events have ever been received).
     */
    virtual void deactivate() = 0;

    /**
     * Execute this block on the given strip.
     *
     * @param   [in/out]    strip           The strip to operate on.
     * @param   [in/out]    noteToLightMap  To map from note number to light number.
     */
    virtual void execute(Processing::TRgbStrip& strip, const Processing::TNoteToLightMap& noteToLightMap) = 0;
};

#endif /* PROCESSING_IPROCESSINGBLOCK_H_ */
