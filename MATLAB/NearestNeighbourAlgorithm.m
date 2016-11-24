training = xlsread('fydptestdata.xlsx', 'A1:E145');
group = xlsread('fydptestdata.xlsx', 'F1:F145');
sample = xlsread('Dummy Data (1).xlsx', 'A1:E145');

gscatter(training(:,1),training(:,2),group,'rb','+x');
legend('Training group 1');
hold on;
% Classify the sample using the nearest neighbor classification
c = knnclassify(sample, training, group);
gscatter(sample(:,1),sample(:,2),c,'mc'); hold on;
legend('Training group 1', 'Training group 2', ...
       'Data in group 1','Data in group 2');
hold off; 