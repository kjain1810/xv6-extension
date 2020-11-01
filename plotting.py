import matplotlib.pyplot as plt

f = open("logs_10_procs_30_aging.txt", "r")
# f = open("plotting.py", "r")
logs = []
for x in f:
    if x[0] == 'L':
        i = 0
        while x[i] != ' ':
            i += 1
        i += 1
        ticks = 0
        while x[i] != ' ':
            ticks *= 10
            ticks += int(x[i])
            i += 1
        i += 1
        proc = 0
        while x[i] != ' ':
            proc *= 10
            proc += int(x[i])
            i += 1
        i += 1
        queue = int(x[i])
        logs.append({"tick": ticks, "proc": proc, "queue": queue})
        print(ticks)
for i in range(4, 15):
    here = []
    for arr in logs:
        if arr["proc"] == i and arr["tick"] < 1000:
            here.append({"ticks": arr["tick"], "queue": arr["queue"]})
    heresorted = sorted(here, key=lambda x: x["ticks"])
    here2_ticks = []
    here2_queue = []
    for arr in heresorted:
        here2_ticks.append(arr["ticks"])
        here2_queue.append(arr["queue"])
    plt.plot(here2_ticks, here2_queue, label="pid: " + str(i))
plt.legend()
plt.show()