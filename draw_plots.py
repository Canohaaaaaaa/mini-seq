import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import pyplot as plt, ticker as mticker
sns.set_theme(style="whitegrid")

hybrid_data = pd.read_csv("./data/csv/hybrid.csv")
copy_copyless_size = pd.read_csv("./data/csv/copy_copyless_size_huge.csv")
heap_data = pd.read_csv("./data/csv/heaps.csv")

copy_copyless_size = copy_copyless_size.set_index("Taille")
hybrid_data = hybrid_data.set_index("Tâche")
### HYBRIDE ###
graph = sns.lineplot(data=hybrid_data[[" Miniseq homogène", " Miniseq hybride", " AFF3CT-hybride", " AFF3CT"]], dashes=False, marker=".")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("N° tâche")
plt.xlim(1, 30)
plt.xticks(hybrid_data.index)
#plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Miniseq homogène"], linewidth=1, yerr=hybrid_data[" Ecart-type Miniseq homogène"], fmt='none', capsize=3, capthick=1, )
#plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Miniseq hybride"], linewidth=1, yerr=hybrid_data[" Ecart-type Miniseq hybride"], fmt='none', capsize=3, capthick=1, color="#FFA07A")
#plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Aff3ct homogène"], linewidth=1, yerr=hybrid_data[" Ecart-type Aff3ct homogène"], fmt='none', capsize=3, capthick=1, color="#D33F49")
#plt.errorbar(x=hybrid_data.index, y=hybrid_data[" Aff3ct hybride"], linewidth=1, yerr=hybrid_data[" Ecart-type Aff3ct hybride"], fmt='none', capsize=3, capthick=1, color="#3fa162")
plt.legend(loc='lower left', fontsize="8")
plt.tick_params(axis='x', which='major', labelsize=8)
graph.get_figure().savefig("./data/graph/hybrid_homog.png")
plt.close()
#### TEST ####
graph = sns.lineplot(data=copy_copyless_size[[" Miniseq", " Miniseq-io", " AFF3CT-io", " AFF3CT"]], dashes=False)
plt.yscale('log')
plt.xscale('log')
plt.savefig("./data/graph/copy_copyless_affect_size_huge_loglog.png")
plt.ylabel("Temps d'execution (µs)")
plt.xlabel("Taille (octet)")
plt.close()