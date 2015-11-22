# Fake Battery Module for the Linux kernel

This is a kernel module I wrote (based mainly on the `test_power` module
included in the Linux kernel source) for simulating multiple batteries on
Linux.

This is my first module, so don't hold me responsible if you use it and it
causes a kernel panic. =)

## Loading the module

You can build the module with a simple `make`, and load it with `insmod`:

    $ sudo insmod ./fake_battery.ko

## Changing battery values via /dev/fake\_battery

You can write values to `/dev/fake_battery` to change the current charging/discharging
and charge levels of the battery:

    $ echo 'charging = 0' | sudo tee /dev/fake_battery # set state to discharging
    $ echo 'charging = 1' | sudo tee /dev/fake_battery # set state to charging
    $ echo 'capacity0 = 77' | sudo tee /dev/fake_battery # set charge on BAT0 to 77%
    $ echo 'capacity1 = 77' | sudo tee /dev/fake_battery # set charge on BAT1 to 77%

