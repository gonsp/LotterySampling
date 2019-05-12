clear all
close all

format long

plot_zipf(1.0001, 50)

function plot_zipf(alpha, n)
    X = [1 : n];
    z = zeta(alpha);
    PDF = [];
    CDF = [];
    for(i = [1 : 50])
        PDF(end + 1) = i^(-alpha) / zeta(alpha);
        aux = 0;
        if(length(CDF) ~= 0)
            aux = CDF(end);
        end
        CDF(end + 1) = aux + PDF(end);
    end
    figure()
    bar(X, PDF);
    ylabel("p(x_j)");
    xlabel("j");
    figure()
    bar(X, CDF);
    ylabel("CDF(x_j)");
    xlabel("j");
    ylim([0, 0.01]);
end
