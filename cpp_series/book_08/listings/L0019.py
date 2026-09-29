from pathlib import Path
s=Path('Dockerfile').read_text()
assert 'COPY --from=builder /work/harbor' in s
assert s.count('FROM ')==2
assert 'USER 10001:10001' in s
print('offline image structure: builder, runtime library, non-root executable')
