statuses=[{'reason':'OOMKilled','exitCode':137},{'reason':'Error','exitCode':137}]
def classify(s):return 'memory-limit investigation' if s.get('reason')=='OOMKilled' else 'cause not established by exit code'
assert classify(statuses[0])!=classify(statuses[1])
for s in statuses:print(classify(s))
