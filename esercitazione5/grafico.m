dati = load('sort_times2.dat');

x = dati(:,1);
bubble = dati(:,2);
insertion = dati(:,3);
selection = dati(:,4);
merge = dati(:,5);
quick = dati(:,6);
quicknuovo = dati(:,7);
sort = dati(:,8);


loglog(x, bubble, 'b')
hold on 
loglog(x, insertion, 'r')
loglog(x, selection, 'g')
loglog(x, merge, 'm')
loglog(x, quick, 'c')
loglog(x, quicknuovo, 'y')
loglog(x, sort, 'k')
hold off

xlabel('Dimensione vettore')
ylabel('Tempo di ordinamento')

legend('Bubble sort', 'Insertion sort', 'Selection sort', 'Merge sort', 'Quick sort', 'Quick nuovo','std::sort')
grid on 