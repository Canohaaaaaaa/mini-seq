import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
sns.set_theme(style="whitegrid")

hybrid_data = pd.read_csv("./data/csv/hybrid.csv")
copy_copyless_length = pd.read_csv("./data/csv/copy_copyless_length.csv")
copy_copyless_size = pd.read_csv("./data/csv/copy_copyless_size.csv")

hybrid_data = hybrid_data.set_index("Tâche")
copy_copyless_length = copy_copyless_length.set_index('Longueur')
copy_copyless_size = copy_copyless_size.set_index('Taille')
### HYBRIDE ###
graph = sns.lineplot(data=hybrid_data, dashes=False, marker=".", errorbar="sd")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("N° tâche")
plt.ylim(0)
plt.title("Temps d'exécution en fonction du N° de la tâche\n [2048 * 4 entiers -01]")
#sns.lineplot(x=hybrid_data["Tâche"], y=hybrid_data[" Séquence homogène"], data=hybrid_data)
#graph.get_figure().savefig("./data/graph/hybrid_homog.png")
plt.close()
### COPY-COPYLESS SIZE ###
graph = sns.lineplot(data=copy_copyless_size[[" Séquence avec copie", " Séquence sans copie"]], dashes=False, marker=".")
plt.errorbar(x=copy_copyless_size.index, y=copy_copyless_size[" Séquence avec copie"], yerr=copy_copyless_size[" Ecart-type copie"], fmt='none', capsize=3, capthick=1, )
plt.errorbar(x=copy_copyless_size.index, y=copy_copyless_size[" Séquence sans copie"], yerr=copy_copyless_size[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
plt.title("Temps d'exécution en fonction de la taille des données\n [20 tâches]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Taille (entiers 4o)")
graph.get_figure().savefig("./data/graph/copy_copyless_size.png")
plt.close()
### COPY-COPYLESS LENGTH ###
graph = sns.lineplot(data=copy_copyless_length[[" Séquence avec copie", " Séquence sans copie"]], dashes=False, marker=".")
plt.errorbar(x=copy_copyless_length.index, y=copy_copyless_length[" Séquence avec copie"], yerr=copy_copyless_length[" Ecart-type copie"], fmt='none', capsize=3, capthick=1, )
plt.errorbar(x=copy_copyless_length.index, y=copy_copyless_length[" Séquence sans copie"], yerr=copy_copyless_length[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
plt.title("Temps d'exécution en fonction de la longueur de la séquence\n [2048 * 20 entiers]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Longueur (nombre de tâches)")
graph.get_figure().savefig("./data/graph/copy_copyless_length.png")
# Create a dataframe with some data
#df = pd.DataFrame({
#    "x": [1, 2, 3, 4, 5],
#    "y": [2.1, 4.2, 6.3, 8.4, 10.5],
#    "std": [0.3, 0.4, 0.5, 0.6, 0.7]
#})
#
## Plot the lineplot with error bars
#sns.lineplot(x="x", y="y", data=df)
#plt.errorbar(x=df["x"], y=df["y"], yerr=df["std"], fmt='none', capsize=3, capthick=1)
#plt.show()