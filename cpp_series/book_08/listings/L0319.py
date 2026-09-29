from datetime import datetime,timezone
rows=[('2026-09-01T12:00:05Z','errors increased'),
      ('2026-09-01T08:00:00-04:00','release applied'),
      ('2026-09-01T12:01:00+00:00','rollback requested')]
parsed=[(datetime.fromisoformat(t.replace('Z','+00:00')).astimezone(timezone.utc),e) for t,e in rows]
for t,e in sorted(parsed):print(t.strftime('%H:%M:%SZ'),e)
