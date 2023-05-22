import matplotlib.pyplot as plt
s = input("File Name : ")

with open(s, 'r') as f:
    data = f.readlines()

x = []
y = []
for d in data:
    d = d.strip().split()
    x.append(float(d[0]))
    y.append(float(d[1]))
# Create scatter plot
plt.scatter(x, y)
plt.title('Scatter plot')
plt.xlabel('Payload')
plt.ylabel('Cumulative RTT')
plt.show()
