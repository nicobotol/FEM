 % Plotting Un-Deformed and Deformed Structure
 close all
 clear all
 vib2_plotdeformed_data0001;
 % Make plot
 figure
 set(gcf,'Name','Deformed')
 subplot(2,1,2)
 hold on
 for e = 1:size(IX,1)
     edof = [2*IX(e,1)-1 2*IX(e,1) 2*IX(e,2)-1 2*IX(e,2)...
         2*IX(e,3)-1 2*IX(e,3) 2*IX(e,4)-1 2*IX(e,4)];
     xx = X(IX(e,1:4),1) + D(edof(1:2:8));
     yy = X(IX(e,1:4),2) + D(edof(2:2:8));
     patch(xx,yy,[1 1 0]);
 end
 title('Deformed')
 axis equal;
 xaxes = get(gca,'xlim');
 yaxes = get(gca,'ylim');
 axis off;
 subplot(2,1,1)
 hold on
 for e = 1:size(IX,1)
     xx = X(IX(e,1:4),1);
     yy = X(IX(e,1:4),2);
     patch(xx,yy,[1 1 0]);
 end
 title('Undeformed')
 axis([min(xaxes(1),min(X(:,1))) max(xaxes(2),max(X(:,1)))...
  min(yaxes(1),min(X(:,2))) max(yaxes(2),max(X(:,2))) ]);
 axis equal;
 axis off;
 hold off
 set(gcf,'color',[ 1  1 1]);
