import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
# Read data from the text file
file_path = "C:/Users/li152/VSCode/ROS_Proj/la-spd.txt"
data = pd.read_csv(file_path, delim_whitespace=True)

# Extract relevant columns
x_values = data.iloc[:, 3]  # 4th column (0-based index)
y_values = data.iloc[:, 4]  # 5th column
value1 = data.iloc[:, 5]    # 6th column
value2 = data.iloc[:, 6]    # 7th column
group_column = data.iloc[:, 4]  # spd

# Create a colormap for grouping
unique_groups = group_column.unique()
colors = plt.cm.viridis(np.linspace(0, 1, len(unique_groups)))

# Create two subplots for the maps
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))

# Plot the first map using the 6th column as values
scatter1 = ax1.scatter(x_values, c=value1, cmap='viridis')
ax1.set_title('error')

# Plot the second map using the 7th column as values
scatter2 = ax2.scatter(x_values, y_values, c=value2, cmap='viridis')
ax2.set_title('error-derive')

# Add colorbar to each plot
cbar1 = plt.colorbar(scatter1, ax=ax1)
cbar1.set_label('error')

cbar2 = plt.colorbar(scatter2, ax=ax2)
cbar2.set_label('error_derive')

# Add legend for groups
for i, group in enumerate(unique_groups):
    group_data = data[data.iloc[:, 4] == group]
    ax1.scatter(
        group_data.iloc[:, 3], group_data.iloc[:, 4],
        label=group, color=colors[i], s=40, edgecolor='k'
    )

ax1.legend(title='Groups', loc='best')

# Show the plots
plt.tight_layout()
plt.show()
