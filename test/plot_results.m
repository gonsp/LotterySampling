% clear all
% close all

results = dir('../test/results/*.csv');


first = true;
for result = fliplr(results')
    
    [date, metric, data] = get_experiment_data(result);

    if first
        date_last_experiment = date;
    end
    first = false;
    
    if ~strcmp(date, date_last_experiment)
        break
    end
%     figure('units','normalized','outerposition',[0 0 1 1])
    figure();
    plots = [];
    algorithm_names = {};
    line_styles = {'.', '+', 'o', 'x', '*', 'square', 'diamond', 'pentagram', 'hexagram', 'none'};
    hold on
    for algorithm_index = 2 : size(data, 2)
        algorithm_plot = plot(data.X, data{:, algorithm_index}, 'marker', line_styles{algorithm_index-1});
        algorithm_name = get_algorithm_name(data.Properties.VariableNames(algorithm_index));
        plots = [plots; algorithm_plot];
        algorithm_names(end + 1) = algorithm_name;
    end
    hold off
    legend(plots, algorithm_names);
    title(result.name)
    ylabel(metric)
    if strcmp(metric, 'recall') || strcmp(metric, 'precision')
        ylim([0, 1])
    end
    set(findall(gca, 'Type', 'Line'),'LineWidth',1);
    
    
end

function [date, metric, data] = get_experiment_data(result)
    date = extractBefore(result.name, 20);
    metric = extractBefore(extractAfter(result.name, 20), '-');
    path = strcat(result.folder, '/', result.name);
    data = readtable(path);
end

function [name] = get_algorithm_name(column_name)
    column_name{1}
    name = extractBetween(column_name{1}, 'xxx_', '_xxx');
    name = strrep(name, '_', ' ');
    name = regexprep(lower(name), '(\<[a-z])', '${upper($1)}');
    name = strrep(name, ' ', '');
end