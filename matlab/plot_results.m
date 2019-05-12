clear all
close all

experiment_results = dir('../test/results/*');
last_experiment_results = experiment_results(end);
path = strcat(last_experiment_results.folder, "/", last_experiment_results.name, "/*.csv");

figure_handle = figure('units','normalized','outerposition',[0 0 1 1]);

metric_result_files = dir(char(path))';
prev_date = metric_result_files(1).date;
plots_by_metric = create_plots(metric_result_files);
while true
    while prev_date == metric_result_files(1).date
        metric_result_files = dir(char(path))';
        drawnow;
        if ~figure_handle.isvalid()
            return
        end
    end
    prev_date = metric_result_files(1).date;
    refresh_plots(plots_by_metric, metric_result_files)
end

function [plots_by_metric] = create_plots(metric_result_files)
    i = 1;
    plots_by_metric = {};
    for metric_result_file = metric_result_files
    
        [experiment_name, metric_name, data] = read_results(metric_result_file);

        plots = [];
        algorithm_names = {};
        line_styles = {'.', '+', 'o', 'x', '*', 'square', 'diamond', 'pentagram', 'hexagram', 'none'};
        create_subplot(i, length(metric_result_files));
        hold on
        for algorithm_index = 2 : size(data, 2)
            algorithm_plot = plot(data{:, 1}, data{:, algorithm_index}, 'marker', line_styles{algorithm_index-1});
            plots = [plots; algorithm_plot];
            algorithm_names(end + 1) = get_column_name(data, algorithm_index);
        end
        hold off

        legend_plots = legend(plots, algorithm_names);
        legend_plots.Location = 'southoutside';

        title(experiment_name)
        xlabel(get_column_name(data, 1));
        ylabel(metric_name)
        if strcmp(metric_name, 'Recall') || strcmp(metric_name, 'Precision') || strcmp(metric_name, 'Weighted recall')
            ylim([0, 1])
        end

        set(findall(gca, 'Type', 'Line'),'LineWidth',1);
        
        plots_by_metric{length(plots_by_metric) + 1} = plots;
        drawnow;
        
        i = i + 1;
    end
end

function refresh_plots(plots_by_metric, metric_result_files)
    i = 1;
    for metric_result_file = metric_result_files
        [~, ~, data] = read_results(metric_result_file);
                    
        plots = plots_by_metric{i};
        for algorithm_index = 2 : size(data, 2)
            plot = plots(algorithm_index-1);
            set(plot, 'xdata', data{:, 1});
            set(plot, 'ydata', data{:, algorithm_index});
        end
        i = i + 1;
    end
    drawnow;
end

function [experiment_name, metric_name, data] = read_results(metric_result_file)
    experiment_name = extractBefore(extractAfter(metric_result_file.name, '-'), '.');
    metric_name = extractBefore(metric_result_file.name, '-');
    metric_name = strrep(metric_name, '_', ' ');
    metric_name(1) = upper(metric_name(1));
    path = strcat(metric_result_file.folder, '/', metric_result_file.name);
    data = readtable(path);
end

function [name] = get_column_name(data, i)
    name = data.Properties.VariableNames(i);
end

function create_subplot(i, total_subplots)
    n = ceil(total_subplots / 3);
    m = ceil(total_subplots / n);
    subplot_tight(n, m, i);
end