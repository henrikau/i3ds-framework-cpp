import i3ds

def main():
    con = i3ds.Context.Create()
    fac = i3ds.ClientFactory.Create(con)
    cam = fac.CreateCamera(11)

    cam.set_state(i3ds.activate)
    cam.set_rate(100000)
    cam.set_exposure(10000, 1.0)
    cam.set_region(True, (600,400,100,100))
    cam.set_flash(True, 128)
    cam.set_pattern(True, 1)
    cam.set_state(i3ds.start)
    cam.set_state(i3ds.stop)
    cam.set_state(i3ds.deactivate)

if __name__ == "__main__":
    main()
