import seaborn as sns
import pandas as pd 
import matplotlib.pyplot as plt
sns.set_theme(style="ticks")

# Load the example dataset for Anscombe's quartet
test = pd.read_csv("data.csv")
test = test.set_index('size')
print(test)
#flights_wide.head()
sns.lineplot(data=test)
plt.show()
# Show the results of a linear regression within each dataset
#sns.relplot(data=df_wide,kind="line")
