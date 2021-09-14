MAX_SPEED = 40e3;
stepper_resolution = 1.8/16.0
steps_in_angle = round(1/stepper_resolution)
64/1.8
32/1.8
16/1.8
4/1.8
2/1.8
1/1.8


desired = [0:10:400];
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(1)
plot(desired, real, desired, desired)

stepper_resolution = 1.8/32.0
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(2)
plot(desired, real, desired, desired)

stepper_resolution = 1.8/64.0
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(3)
plot(desired, real, desired, desired)


desired = [0:10:14222];
real = MAX_SPEED./round(MAX_SPEED./desired);
figure(4)
plot(desired, real, desired, desired)

