 % Plotting Element Values
 close all
 clear all
 cantilever5_plotelements_data_0002;
 % Determine colorscale
 colormap('jet')
 cmap = jet;
 cinterp = linspace(min(plotval),max(plotval),size(cmap,1));
 % Make plot
 title('Stresses')
 hold on
 for e = 1:size(IX,1)
    [dummy,arr_pos] = min(abs(cinterp-plotval(e)));
    xx = X(IX(e,1:4),1);
    yy = X(IX(e,1:4),2);
    patch(xx,yy,cmap(arr_pos,:));
 end
 axis equal;
 axis off;
 caxis([min(plotval) max(plotval)]);
 colorbar;
 hold off
 set(gcf,'color',[ 1  1 1]);
