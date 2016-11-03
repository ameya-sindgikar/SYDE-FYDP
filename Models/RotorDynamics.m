% File name:   Example_05_09_01.m
%
% Example 5.9.1
%
% This example has isotropic bearings
% A model with 6 Timoshenko beam elements
%
clear
close all
% Set the material parameters
E = 211e9;
G = 81.2e9;
rho = 7810;
damping_factor = 0;   % no damping in shaft
% Consider a model with 6 equal length elements
% Shaft is 1.5m long
model.node = [1 0.0; 2 0.25; 3 0.5; 4 0.75; 5 1.0; 6 1.25; 7 1.50];
% Assume shaft type 2 - Timoshenko with gyroscopic effects included % Solid shaft with 50mm outside diameter
shaft_od = 0.05;
shaft_id = 0.0;
model.shaft = [2
               2
               2
               2
               2
               2
% Disk 1 at node
% Disk 2 at node
% Note inside diameter of disk is assumed to be the outside diameter % of the shaft
disk1_od = 0.28;
disk2_od = 0.35;
disk_thick = 0.07;
model.disc = [1 3 rho disk_thick disk1_od shaft_od; ...
              1 5 rho disk_thick disk2_od shaft_od];
% Constant stiffness short isotropic bearing (1NM/m) with no damping % Bearings at the ends of the shaft - nodes 1 and 7
bear_stiff = 1e6;
model.bearing = [3 1 bear_stiff bear_stiff 0 0; ...
                 3 7 bear_stiff bear_stiff 0 0];
1 2 shaft_od shaft_id rho E G damping_factor; ... 2 3 shaft_od shaft_id rho E G damping_factor; ... 3 4 shaft_od shaft_id rho E G damping_factor; ... 4 5 shaft_od shaft_id rho E G damping_factor; ... 5 6 shaft_od shaft_id rho E G damping_factor; ... 6 7 shaft_od shaft_id rho E G damping_factor];
3 has diameter of 280mm and thickness of 70mm
5 has diameter of 350mm and thickness of 70mm
18
% Draw the rotor
figure(1), clf
picrotor(model)
% Plot the Campbell diagram
% =========================
% Define the rotor spin speed range
Rotor_Spd_rpm = 0:100:4500.0;
Rotor_Spd = 2*pi*Rotor_Spd_rpm/60; % convert to rad/s
% Calculate the eigensystem for the range of rotor spin speeds
[eigenvalues,eigenvectors,kappa] = chr_root(model,Rotor_Spd);
% Plot Campbell diagram
figure(2)
NX = 2;
damped_NF = 1; % plot damped natural frequencies plotcamp(Rotor_Spd,eigenvalues,NX,damped_NF,kappa)
% Plot the modes and orbits at 4000 rev/min % =========================================
% Calculate the eigensystem at 4000 rev/min
Rotor_Spd_rpm = 4000;
Rotor_Spd = 2*pi*Rotor_Spd_rpm/60; % convert to rad/s [eigenvalues,eigenvectors,kappa] = chr_root(model,Rotor_Spd);
% Plot the first 4 eigenvectors and annotate with corresponding % eigenvalue
figure(3)
subplot(221)
plotmode(model,eigenvectors(:,1),eigenvalues(1)) subplot(222) plotmode(model,eigenvectors(:,3),eigenvalues(3)) subplot(223) plotmode(model,eigenvectors(:,5),eigenvalues(5)) subplot(224) plotmode(model,eigenvectors(:,7),eigenvalues(7))
% Plot the orbits at the disks for the first 6 eigenvectors
% Note that the axes command is used here for different subplots - % the MATLAB command subplot could also have been used
figure(4)
outputnode = [3 5];
axes('position',[0.2 0.53 0.2 0.2 ]) plotorbit(eigenvectors(:,1),outputnode,'Mode 1',eigenvalues(1)) axes('position',[0.39 0.53 0.2 0.2 ]) plotorbit(eigenvectors(:,3),outputnode,'Mode 2',eigenvalues(3)) axes('position',[0.58 0.53 0.2 0.2 ]) plotorbit(eigenvectors(:,5),outputnode,'Mode 3',eigenvalues(5)) axes('position',[0.2 0.25 0.2 0.2 ]) plotorbit(eigenvectors(:,7),outputnode,'Mode 4',eigenvalues(7)) axes('position',[0.39 0.25 0.2 0.2 ]) plotorbit(eigenvectors(:,9),outputnode,'Mode 5',eigenvalues(9)) axes('position',[0.58 0.25 0.2 0.2 ]) plotorbit(eigenvectors(:,11),outputnode,'Mode 6',eigenvalues(11))
