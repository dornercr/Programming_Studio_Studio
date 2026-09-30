import pathlib,sqlite3,tempfile
with tempfile.TemporaryDirectory() as td:
 root=pathlib.Path(td)
 with sqlite3.connect(root/'live.db') as live:
  live.execute('CREATE TABLE jobs(id TEXT PRIMARY KEY, result INTEGER NOT NULL)')
  live.execute('INSERT INTO jobs VALUES(?,?)',('a',49));live.commit()
  with sqlite3.connect(root/'backup.db') as backup:
   live.backup(backup)
   assert backup.execute('SELECT result FROM jobs WHERE id=?',('a',)).fetchone()==(49,)
  live.execute('UPDATE jobs SET result=64 WHERE id=?',('a',));live.commit()
 with sqlite3.connect(root/'backup.db') as restored:
  assert restored.execute('SELECT result FROM jobs').fetchone()==(49,)
 print('backup preserves committed result=49 independently of later live update=64')
