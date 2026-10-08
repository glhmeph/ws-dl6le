#ifndef AUTO_CQ_STATE_HPP
#define AUTO_CQ_STATE_HPP

#include <cstdint>

// Completion is requested while the final message is being prepared. Do not
// permit another transmission until its PTT tail and any logging have finished.
class AutoCQState
{
public:
  bool pending () const { return pending_; }

  void request (std::int64_t now, std::int64_t period, bool transmitting)
  {
    if (pending_ || period <= 0) return;
    pending_ = true;
    transmission_ended_ = !transmitting;
    earliest_ = nextPeriod (now, period);
  }

  void transmissionEnded (std::int64_t now, std::int64_t period)
  {
    if (!pending_ || transmission_ended_ || period <= 0) return;
    transmission_ended_ = true;
    earliest_ = nextPeriod (now, period);
  }

  bool ready (std::int64_t now, bool tx_busy, bool log_busy) const
  {
    return pending_ && transmission_ended_ && !tx_busy && !log_busy && now >= earliest_;
  }

  void cancel () { pending_ = false; transmission_ended_ = false; }

private:
  static std::int64_t nextPeriod (std::int64_t now, std::int64_t period)
  {
    return (now / period + 1) * period;
  }
  bool pending_ {false};
  bool transmission_ended_ {false};
  std::int64_t earliest_ {0};
};

#endif
