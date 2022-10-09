 % Plotting Vector Field, i.e. principle stresses
 close all
 clear all
 test_plotvector_data0003;
 % Make plot
 % User scale parameter: See fedata - scale_vec
 scale_vec = 1;
 % Define characteristic length
 clx = 0;   cly = 0;
 for e = 1:size(IX,1)
     for i = 1:4
         for j = i+1:4
             clx = max(abs(  X(IX(e,i),1) - X(IX(e,j),1)   ),clx);
             cly = max(abs(  X(IX(e,i),2) - X(IX(e,j),2)   ),cly);
         end
     end
 end
 clmax = max(clx, cly);
 scal = max(max(abs(vect(:,1:2))))*sqrt(10)/clmax / scale_vec;
 % Make plot
 figure
 hold on
 for e = 1:size(IX,1)
     xx = X(IX(e,1:4),1);
     yy = X(IX(e,1:4),2);
     patch(xx,yy,[1 1 1]);
     % Find approx. center for isoparametric element
     xc = sum(X(IX(e,:),1))/size(IX,2);
     yc = sum(X(IX(e,:),2))/size(IX,2);
     % Directions for vect(:)
     vec = [cos(-vect(e,3)) sin(-vect(e,3)) ...
         cos(-vect(e,3)+pi/2) sin(-vect(e,3)+pi/2)];
     % Plot magnitude and direction of vect_1
     cc = 'b';
     if vect(e,1) < 0,    cc = 'r';     end
     quiver(xc,yc,vec(1),vec(2),abs(vect(e,1))/scal,cc)
     quiver(xc,yc,-vec(1),-vec(2),abs(vect(e,1))/scal,cc)
     % Plot magnitude and direction of vect_2
     cc = 'b';
     if vect(e,2) < 0,    cc = 'r';     end
     quiver(xc,yc,vec(3),vec(4),abs(vect(e,2))/scal,cc)
     quiver(xc,yc,-vec(3),-vec(4),abs(vect(e,2))/scal,cc)
 end   
 title( 'Principal')
 axis equal;  axis off;  hold off
 set(gcf,'color',[ 1  1  1]);
