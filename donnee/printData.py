import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
sns.set_theme(style="ticks")

# Load the example dataset for Anscombe's quartet
df = pd.read_csv("data.csv")
print(df)


# Show the results of a linear regression within each dataset
sns.lineplot(data=df,x="seq_size",y="tempsCopy")
ax = sns.lineplot(data=df,x="seq_size",y="tempsSansCopy")
ax.set(xlabel='taille sequence', ylabel="temps d'executon",title="Copy Vs CopyLess")
ax.legend(loc="upper left")
plt.show()