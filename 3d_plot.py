import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Read data from the text file
file_path = "C:/Users/li152/VSCode/ROS_Proj/la-spd.txt"
data = pd.read_csv(file_path, delim_whitespace=True)

# Extract relevant columns
look_ahead_coeff = data.iloc[:, 3]  # 4th column (0-based index)
speed_limit = data.iloc[:, 4]  # 5th column
error = data.iloc[:, 5]    # 6th column
error_derive = data.iloc[:, 6]
time = data.iloc[:, 7]
kp = data.iloc[:, 0]  # 1st column for grouping

# Create a colormap for grouping

# Create a 3D scatter plot
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot the data points in 3D space with different colors for each group

ax.scatter(kp, look_ahead_coeff, error_derive)

ax.set_xlabel('Kp')
ax.set_ylabel('look ahead coeff')
ax.set_zlabel('error_derive')
ax.set_title('3D Scatter Plot of error_derive')

# Add legend for groups
ax.legend(title='speed_limit = 2.0', loc='best')

# Show the plot
plt.show()
