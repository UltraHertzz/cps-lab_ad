import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
# Read data from the text file
file_path = "C:/Users/li152/VSCode/ROS_Proj/kp_LA.txt"
data = pd.read_csv(file_path, delim_whitespace=True)

# Extract relevant columns
x = data['LA']
values = [data['error'], data['error_derive'], data['TIME']]
colors = data['Kp']
dic = ['error','error_dirive','TIME']
# Create subplots
fig, axs = plt.subplots(1, 3, figsize=(15, 5))

# Create a colormap for coloring based on the 'LA' column
colormap = plt.cm.get_cmap('viridis', len(np.unique(colors)))

# Plot each value with a unique color
for i, ax in enumerate(axs):
    for j, color_value in enumerate(np.unique(colors)):
        mask = (colors == color_value)
        ax.plot(x[mask], values[i][mask], label=f'Kp={color_value}', color=colormap(j / len(np.unique(colors))))

    ax.set_xlabel('look ahead coefficience')
    ax.set_ylabel(dic[i])
    ax.legend()

# Set a common title for the entire figure
fig.suptitle('speed = 3')

# Display the plots
plt.tight_layout()
plt.show()
