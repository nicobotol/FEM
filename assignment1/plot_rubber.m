clear
close all
clc

step_size = 0.05;

rubber_param = [  1.2 5 0.2 50; % material 1
                  0 1 0.2 200]; % material 2
step = [0:step_size/100:abs(step_size-1)];

figure()
for i=1:2
  rubber = rubber_param(i,:);

  for i=1:size(step,2)
    load(i) = Etfunction(step(i), rubber);
  end

  plot(step, load)
  hold on
end
legend('Es1', 'Es2')

function [Et] = Etfunction(epsilon, rubber_param)
  c1 = rubber_param(1);
  c2 = rubber_param(2);
  c3 = rubber_param(3);
  c4 = rubber_param(4);

  Et = c4*(c1*(1 + 2*(1 + c4*epsilon)^(-3)) + 3*c2*(1 + c4*epsilon)^(-4) + 3*c3*(-1 + (1 + c4*epsilon)^2 - 2*(1 + c4*epsilon)^(-3) + 2*(1 + c4*epsilon)^(-4)));
end