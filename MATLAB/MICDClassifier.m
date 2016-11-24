%declare MICD classifier function
function value = MICDClassifier(mu, sigma, x)
    
    %declare the number of classes
    numClasses = 10;
    mini = 10000;
    
    %find the decision criteria
    for i = 1:numClasses
        dMICD(i) = sqrt((x - mu(:, i))'*(inv(sigma(:, :, i)))*(x - mu(:, i)));

    end
    
    [t, MICD] = min(dMICD);
    value = MICD;
end
