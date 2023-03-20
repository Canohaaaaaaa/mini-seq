set terminal png size 800,800
set output 'evoltion_sequence_task_size.png'
set title "evolution sequence(seq 100) en fonction de l'entree"
set xlabel 'taill entree'
set ylabel "temps d'execution (us)"
plot "evolution_sequence_tache_size.dat" using 1:2 title 'avec copie' with lines , \
                               "evolution_sequence_tache_size.dat" using 1:3 title 'sans copie' with lines