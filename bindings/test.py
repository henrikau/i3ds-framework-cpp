import i3ds

def test_camera(factory):
    s = factory.Camera(11)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_exposure(10000, 1.0)
    s.set_region(True, (600,400,100,100))
    s.set_flash(True, 128)
    s.set_pattern(True, 1)
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def test_tof_camera(factory):
    s = factory.ToFCamera(13)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_region(True, (200,200,100,100))
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def test_lidar(factory):
    s = factory.LIDAR(14)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_region(True, (0.3,-0.4,0.1,0.2))
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def test_radar(factory):
    s = factory.Radar(15)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_region(True, (10,20,30,10))
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def test_star_tracker(factory):
    s = factory.StarTracker(16)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def test_imu(factory):
    s = factory.IMU(17)
    s.set_state(i3ds.activate)
    s.set_sampling(100000)
    s.set_state(i3ds.start)
    s.set_state(i3ds.stop)
    s.set_state(i3ds.deactivate)    

def main():
    connection = i3ds.Context.Create()
    factory = i3ds.ClientFactory.Create(connection)
    test_camera(factory)
    test_tof_camera(factory)
    test_lidar(factory)
    test_radar(factory)
    test_star_tracker(factory)
    test_imu(factory)

if __name__ == "__main__":
    main()
