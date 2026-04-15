dati = load('sort_times.dat');

x = dati(:,1);
bubble = dati(:,2);
insertion = dati(:,3);
selection = dati(:,4);
sort = dati(:,5);

loglog(x, bubble, 'b')
hold on 
loglog(x, insertion, 'r')
loglog(x, selection, 'g')
loglog(x, sort, 'k')
hold off

xlabel('Dimensione vettore')
ylabel('Tempo di ordinamento')

legend('Bubble sort', 'Insertion sort', 'Selection sort', 'std::sort')
grid on 