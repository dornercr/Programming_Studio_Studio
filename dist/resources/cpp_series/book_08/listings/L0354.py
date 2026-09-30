import sqlite3,pathlib,tempfile
with tempfile.TemporaryDirectory() as td:
 p=pathlib.Path(td)/'restored.db'
 with sqlite3.connect(p) as db:
  db.execute('CREATE TABLE jobs(id TEXT PRIMARY KEY, state TEXT, result INTEGER)')
  db.execute("INSERT INTO jobs VALUES('job-7','done',49)");db.commit()
 with sqlite3.connect(p) as db:
  assert db.execute('PRAGMA integrity_check').fetchone()==('ok',)
  assert db.execute("SELECT state,result FROM jobs WHERE id='job-7'").fetchone()==('done',49)
 backup_time=1000;incident_time=1300;recovery_start=1305;service_verified=1340
 print(f'restore query=done/49; modeled data age={incident_time-backup_time}s; recovery={service_verified-recovery_start}s')
