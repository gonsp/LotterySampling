clear all
close all

format long

SEED = 4;

N = 100;
h = 100;

tickets = zeros(N, h);
max_tickets = zeros(N, h);
expected_tickets = zeros(N, 1);

for i = 1 : N
    for j = 1 : h
        tickets(i, j) = rand();
        if i == 1
            max_tickets(i, j) = tickets(i, j);
        else
            max_tickets(i, j) = max(tickets(i, j), max_tickets(i-1, j));
        end
    end
    expected_tickets(i) = get_expected_ticket(i);
end

averaged_max_tickets = mean(max_tickets, 2);
variance_max_tickets = std(max_tickets, 0, 2);

hold on
plot(expected_tickets)
plot(averaged_max_tickets)
plot(variance_max_tickets)
% plot(max_tickets, 'r')