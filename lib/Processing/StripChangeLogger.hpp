#ifndef PROCESSING_STRIPCHANGELOGGER_H_
#define PROCESSING_STRIPCHANGELOGGER_H_

#include <mutex>

#include "Concert.hpp"

/**
 * Class which logs every strip update if it's different from the previous one.
 */
class StripChangeLogger : public Concert::IObserver
{
  public:
    explicit StripChangeLogger(Concert& concert);

    StripChangeLogger() = delete;
    StripChangeLogger(const StripChangeLogger&) = delete;
    StripChangeLogger& operator=(const StripChangeLogger&) = delete;

    virtual ~StripChangeLogger();

    // Concert::IObserver implementation
    void onStripUpdate(const Processing::TRgbStrip& strip) override;

  private:
    Concert& concert;
    Processing::TRgbStrip previous;
    mutable std::mutex mutex;
};

#endif /* PROCESSING_STRIPCHANGELOGGER_H_ */
