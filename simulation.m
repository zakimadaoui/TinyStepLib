MAX_SPEED = 40e3;
stepper_resolution = 1.8/16.0;
steps_in_angle = round(1/stepper_resolution);

% observe steps drift with more resolution
64/1.8
32/1.8
16/1.8
4/1.8
2/1.8
1/1.8

% simulate deg/sec
desired = [0:400];

stepper_resolution = 1.8/4;
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(1);
plot(desired, real, desired,desired);
title('Actual vs Desired speed with 1/4 micro-stepping');
legend({'actual','desired'},'Location','southeast');
ylabel('Speed in Degrees/sec') ;



stepper_resolution = 1.8/16;
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(2);
plot(desired, real, desired,desired);
title('Actual vs Desired speed with 1/16 micro-stepping');
legend({'actual','desired'},'Location','southeast');
ylabel('Speed in Degrees/sec') ;



stepper_resolution = 1.8/32;
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(3)
plot(desired, real, desired,desired);
title('Actual vs Desired speed with 1/32 micro-stepping');
legend({'actual','desired'},'Location','southeast');
ylabel('Speed in Degrees/sec') ;



stepper_resolution = 1.8/64;
real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
figure(4)
plot(desired, real, desired,desired);
title('Actual vs Desired speed with 1/64 micro-stepping');
legend({'actual','desired'},'Location','southeast');
ylabel('Speed in Degrees/sec') ;



% this simulates steps/sec instead of dergrees/sec
desired = [0:3000];
real = MAX_SPEED./round(MAX_SPEED./desired);
figure(5)
plot(desired, real, desired, desired)
title('Actual vs Desired speed in Steps/sec ');
legend({'actual','desired'},'Location','southeast');
ylabel('Speed in Step/sec') ;

