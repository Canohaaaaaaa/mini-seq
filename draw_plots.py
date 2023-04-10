import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
sns.set_theme(style="whitegrid")

hybrid_data = pd.read_csv("./data/csv/hybrid.csv")
copy_copyless_length = pd.read_csv("./data/csv/copy_copyless_length.csv")
copy_copyless_size = pd.read_csv("./data/csv/copy_copyless_size.csv")
copy_copyless_affect_size_full = pd.read_csv("./data/csv/copy_copy_less_affect.csv")

hybrid_data = hybrid_data.set_index("Tâche")
copy_copyless_length = copy_copyless_length.set_index('Longueur')
copy_copyless_size = copy_copyless_size.set_index('Taille')
copy_copyless_affect_size_full = copy_copyless_affect_size_full.set_index('Taille')
### HYBRIDE ###
graph = sns.lineplot(data=hybrid_data[[" Séquence homogène", " Séquence hybride"]], dashes=False, marker=".")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("N° tâche")
plt.xlim(1, 30)
plt.xticks(hybrid_data.index)
plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Séquence homogène"], linewidth=1, yerr=hybrid_data[" Ecart-type homogène"], fmt='none', capsize=3, capthick=1, )
plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Séquence hybride"], linewidth=1, yerr=hybrid_data[" Ecart-type hybride"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
plt.title("Temps d'exécution en fonction du N° de la tâche\n [buffer: 32 ko]")
plt.tick_params(axis='x', which='major', labelsize=8)
graph.get_figure().savefig("./data/graph/hybrid_homog.png")
plt.close()
### COPY-COPYLESS LENGTH ###
graph = sns.lineplot(data=copy_copyless_length[[" Séquence avec copie", " Séquence sans copie"]], dashes=False, marker=".")
plt.errorbar(x=copy_copyless_length.index, y=copy_copyless_length[" Séquence avec copie"], linewidth=1, yerr=copy_copyless_length[" Ecart-type copie"], fmt='none', capsize=3, capthick=1, )
plt.errorbar(x=copy_copyless_length.index, y=copy_copyless_length[" Séquence sans copie"], linewidth=1, yerr=copy_copyless_length[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
plt.title("Temps d'exécution en fonction de la longueur de la séquence\n [buffer: 150 ko]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Longueur (nombre de tâches)")
plt.xticks(copy_copyless_length.index)
plt.ylim(0)
plt.xlim(1, 19)
graph.get_figure().savefig("./data/graph/copy_copyless_length.png")
plt.close()
### COPY-COPY-LESS-AFFECT ###
copy_copyless_affect_size = copy_copyless_affect_size_full.loc[(copy_copyless_affect_size_full.index >= 32) & (copy_copyless_affect_size_full.index <= 83)]
graph = sns.lineplot(data=copy_copyless_affect_size[[" Séquence avec copie", " Séquence sans copie", " Aff3ct"]], dashes=False, marker=".")
plt.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Séquence avec copie"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type copie"], fmt='none', capsize=3, capthick=1)
plt.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Aff3ct"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type Aff3ct"], fmt='none', capsize=3, capthick=1, color="#4CAF50")
plt.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Séquence sans copie"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")

plt.title("Temps d'exécution en fonction de la taille des données\n [20 tâches]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Taille (ko)")
plt.xlim(32, 83)
plt.xticks(copy_copyless_affect_size.index)
graph.get_figure().savefig("./data/graph/copy_copyless_affect_size.png")
plt.close()
### COPY-COPY-LESS-AFFECT FULL ###
graph = sns.lineplot(data=copy_copyless_affect_size_full[[" Séquence avec copie", " Séquence sans copie", " Aff3ct"]], dashes=False, marker=".")
plt.errorbar(x=copy_copyless_affect_size_full.index, y=copy_copyless_affect_size_full[" Séquence avec copie"], linewidth=1, yerr=copy_copyless_affect_size_full[" Ecart-type copie"], fmt='none', capsize=3, capthick=1)
plt.errorbar(x=copy_copyless_affect_size_full.index, y=copy_copyless_affect_size_full[" Aff3ct"], linewidth=1, yerr=copy_copyless_affect_size_full[" Ecart-type Aff3ct"], fmt='none', capsize=3, capthick=1, color="#4CAF50")
plt.errorbar(x=copy_copyless_affect_size_full.index, y=copy_copyless_affect_size_full[" Séquence sans copie"], linewidth=1, yerr=copy_copyless_affect_size_full[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")

plt.title("Temps d'exécution en fonction de la taille des données\n [20 tâches]")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Taille (ko)")
plt.xlim(2, 99)
plt.ylim(0)
plt.xticks(copy_copyless_affect_size_full.index)
graph.get_figure().savefig("./data/graph/copy_copyless_affect_size_full.png")
plt.close()

### COPY-COPY-LESS-AFFECT GAIN ###
copy_copyless_affect_size = copy_copyless_affect_size_full.loc[(copy_copyless_affect_size_full.index >= 32) & (copy_copyless_affect_size_full.index <= 99)]
gain_copy_copy_less = (copy_copyless_affect_size[" Séquence sans copie"] - copy_copyless_affect_size[" Séquence avec copie"]) / copy_copyless_affect_size[" Séquence avec copie"] * 100 * (-1) 
gain_copy_affect = (copy_copyless_affect_size[" Séquence avec copie"] - copy_copyless_affect_size[" Aff3ct"]) / copy_copyless_affect_size[" Aff3ct"] * 100 * (-1)
gain_copy_less_affect = (copy_copyless_affect_size[" Séquence sans copie"] - copy_copyless_affect_size[" Aff3ct"]) / copy_copyless_affect_size[" Aff3ct"] * 100 * (-1)

fig, ax1 = plt.subplots(figsize=(12,6))
ax2 = ax1.twinx()

ax1.set_ylabel("Temps d'execution (µs)")
ax1.set_xlabel("Taille (ko)")
ax1.set_ylim(0, 200)
ax1.set_title("Temps d'exécution en fonction de la taille des données\n [20 tâches]")

ax2.set_ylabel("Gain (%)")
ax2.set_ylim(0, 100)
ax2.grid(False)
# plot the second graph with "copy-copyless-affect" data
sns.lineplot(data=copy_copyless_affect_size[[" Séquence avec copie", " Séquence sans copie", " Aff3ct"]], dashes=False, marker=".", ax=ax1)
ax1.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Séquence avec copie"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type copie"], fmt='none', capsize=3, capthick=1)
ax1.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Aff3ct"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type Aff3ct"], fmt='none', capsize=3, capthick=1, color="#4CAF50")
ax1.errorbar(x=copy_copyless_affect_size.index, y=copy_copyless_affect_size[" Séquence sans copie"], linewidth=1, yerr=copy_copyless_affect_size[" Ecart-type sans copie"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
ax2.bar(copy_copyless_affect_size.index+0.5, gain_copy_copy_less, width=1, label="Avec Copie | Sans Copie", color="grey")
ax2.bar(copy_copyless_affect_size.index+1.5, gain_copy_affect, width=1, label="Avec Copie | Aff3ct", color="brown")
ax2.bar(copy_copyless_affect_size.index+2.5, gain_copy_less_affect, width=1,  label="Sans Copie | Aff3ct", color="olivedrab")
ax2.legend()
plt.xticks(copy_copyless_affect_size.index)
plt.xlim(32, 88)
plt.savefig("./data/graph/copy_copyless_affect_size_gain.png")
plt.close()