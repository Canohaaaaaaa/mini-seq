import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
sns.set_theme(style="whitegrid")

hybrid_data = pd.read_csv("./data/csv/hybrid.csv")
copy_copyless_length = pd.read_csv("./data/csv/copy_copyless_length.csv")
copy_copyless_size = pd.read_csv("./data/csv/copy_copyless_size.csv")

hybrid_data = hybrid_data.set_index('Tâche')
copy_copyless_length = copy_copyless_length.set_index('Longueur')
copy_copyless_size = copy_copyless_size.set_index('Taille')
### HYBRIDE ###
graph = sns.lineplot(data=hybrid_data, dashes=False, marker=".", errorbar="sd")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("N° tâche")
plt.ylim(0)
plt.title("Temps d'exécution en fonction du N° de la tâche\n [2048 * 4 entiers -01]")
graph.get_figure().savefig("./data/graph/hybrid_homog.png")
plt.close()
### COPY-COPYLESS SIZE ###
graph = sns.lineplot(data=copy_copyless_size, dashes=False, marker=".")
plt.title("Temps d'exécution en fonction de la taille des données\n [20 tâches]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Taille (entiers 4o)")
graph.get_figure().savefig("./data/graph/copy_copyless_size.png")
plt.close()
### COPY-COPYLESS LENGTH ###
graph = sns.lineplot(data=copy_copyless_length, dashes=False, marker=".")
plt.title("Temps d'exécution en fonction de la longueur de la séquence\n [2048 * 20 entiers]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Longueur (nombre de tâches)")
graph.get_figure().savefig("./data/graph/copy_copyless_length.png")