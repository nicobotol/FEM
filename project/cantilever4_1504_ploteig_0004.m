 % Plotting Eigenmodes
 close all
 clear all
 cantilever4_1504_ploteig_data_0004;
 freq =    3918.1220046646031      ;
 timeint =   0.10000000000000001      ;
 timetot =    10.000000000000000      ;
 % Find max window size.
 lxmin = min(X(:,1));        lxmax = max(X(:,1));
 lymin = min(X(:,2));        lymax = max(X(:,2));
 dxmin = min(D(1:2:end));    dxmax = max(D(1:2:end));
 dymin = min(D(2:2:end));    dymax = max(D(2:2:end));
 lxmin = lxmin - max(abs(dxmin),abs(dxmax))*1.05;
 lxmax = lxmax + max(abs(dxmin),abs(dxmax))*1.05;
 lymin = lymin - max(abs(dymin),abs(dymax))*1.05;
 lymax = lymax + max(abs(dymin),abs(dymax))*1.05;
 % Make plot
 figure
 set(gcf,'color',[ 1  1 1]);
 times = 0:timeint:timetot;
 for i = 1:length(times)
     tfact = sin(freq*times(i));
     clf;
     hold on
     for e = 1:size(IX,1)
        edof = [2*IX(e,1)-1 2*IX(e,1) 2*IX(e,2)-1 2*IX(e,2)...
           2*IX(e,3)-1 2*IX(e,3) 2*IX(e,4)-1 2*IX(e,4)];
        xx = X(IX(e,1:4),1) + tfact*D(edof(1:2:8));
        yy = X(IX(e,1:4),2) + tfact*D(edof(2:2:8));
        patch(xx,yy,[1 1 0]);
     end
     axis([lxmin lxmax lymin lymax])
     axis off
     title( 'Mode Shape')
     pause(0.01)
 end
