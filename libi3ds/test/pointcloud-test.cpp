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
  std::uniform_int_distribution<> dis_int(100, 1000);

  size_t n_nums = dis_int(gen);
  std::vector<Point> points;
  points.reserve(n_nums);
  for (size_t i=0; i < n_nums; i++)
    {
      Point p;
      p.arr[0] = dis_real(gen);
      p.arr[1] = dis_real(gen);
      p.arr[2] = dis_real(gen);
      points.push_back(p);
    }

  BOOST_CHECK_GT(points.size(), 99);
  BOOST_CHECK_LT(points.size(), 1001);

  PointCloud pc;
  pc.append_points(points);
  BOOST_CHECK_EQUAL(points.size(), pc.numPoints);

  Message message;
  Encode<PointCloudCodec>(message, pc);
  BOOST_CHECK_EQUAL(message.payloads(), 2);

  PointCloud pc2;
  Decode<PointCloudCodec>(message, pc2);
  BOOST_CHECK_EQUAL(pc.numPoints, pc2.numPoints);
  BOOST_CHECK_EQUAL(pc2.numPoints, n_nums);

  auto const points2 = pc2.points();
  for (size_t i=0; i < n_nums; i++)
    {
      const auto p = points[i];
      const auto p2 = points2[i];
      for (size_t j=0; j < 3; j++)
        {
          BOOST_CHECK_EQUAL(p.arr[j], p2.arr[j]);
        }
    }

  BOOST_CHECK_EQUAL(points[0].arr[0], points2[0].arr[0]);
}
