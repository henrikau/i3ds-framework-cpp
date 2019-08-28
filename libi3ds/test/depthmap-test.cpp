#define BOOST_TEST_MODULE depthmap_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <random>

#include <i3ds/depthmap.hpp>

using namespace i3ds;

BOOST_AUTO_TEST_CASE(depthmap_encdec)
{
  std::random_device r;
  std::default_random_engine gen(r());
  std::uniform_real_distribution<> dis_real(0.0, 1.0);

  size_t n_nums = 1000;

  DepthMap map;

  DepthMapCodec::Initialize(map);
  
  map.depths.reserve(n_nums);

  for (size_t i=0; i < n_nums; i++)
    {
      map.depths.push_back(dis_real(gen));
    }

  BOOST_CHECK_EQUAL(map.depths.size(), 1000);

  Message message;
  Encode<DepthMapCodec>(message, map);
  BOOST_CHECK_EQUAL(message.payloads(), 2);

  DepthMap map2;
  Decode<DepthMapCodec>(message, map2);

  BOOST_CHECK_EQUAL(map.depths.size(), map2.depths.size());

  for (size_t i = 0; i < n_nums; i++)
    {
      const auto d = map.depths[i];
      const auto d2 = map2.depths[i];
      
      BOOST_CHECK_EQUAL(d, d2);
    }
}
