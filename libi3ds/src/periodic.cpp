///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/periodic.hpp>

i3ds::Sampler::Sampler(Operation operation)
  : operation_(operation), period_(0), running_(false)
{
}

i3ds::Sampler::~Sampler()
{
  Stop();
}

void
i3ds::Sampler::Start(SamplePeriod period)
{
  period_ = to_duration(period);
  running_ = true;

  worker_ = std::thread(&i3ds::Sampler::Run, this);
}

void
i3ds::Sampler::Stop()
{
  running_ = false;

  if (worker_.joinable())
    {
      worker_.join();
    }
}

void
i3ds::Sampler::Run()
{
  bool more = true;
  clock::time_point next = clock::now();

  while (running_ && more)
    {
      more = operation_(to_timestamp(next));

      next += period_;
      std::this_thread::sleep_until(next);
    }
}
