#define BOOST_TEST_MODULE pointcloud_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <random>

#include <i3ds/pointcloud.hpp>

using namespace i3ds;

BOOST_AUTO_TEST_CASE(pointcloud_encdec)
{
  std::random_device r;
  std::default_random_engine gen(r());
  std::uniform_real_distribution<> dis_real(0.0, 1.0);

  size_t n_nums = 1000;

  PointCloud pc;

  PointCloudCodec::Initialize(pc);
  
  pc.points.reserve(n_nums);

  for (size_t i=0; i < n_nums; i++)
    {
      PointXYZ p;

      p.x = dis_real(gen);
      p.y = dis_real(gen);
      p.z = dis_real(gen);

      pc.points.push_back(p);
    }

  BOOST_CHECK_EQUAL(pc.points.size(), 1000);

  Message message;
  Encode<PointCloudCodec>(message, pc);
  BOOST_CHECK_EQUAL(message.payloads(), 2);

  PointCloud pc2;
  Decode<PointCloudCodec>(message, pc2);

  BOOST_CHECK_EQUAL(pc.points.size(), pc2.points.size());

  for (size_t i = 0; i < n_nums; i++)
    {
      const auto p = pc.points[i];
      const auto p2 = pc2.points[i];
      
      BOOST_CHECK_EQUAL(p.x, p2.x);
      BOOST_CHECK_EQUAL(p.y, p2.y);
      BOOST_CHECK_EQUAL(p.z, p2.z);
    }
}
