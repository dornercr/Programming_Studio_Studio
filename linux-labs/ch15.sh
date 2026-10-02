#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf '#!/usr/bin/env bash\n: "${REPORT_DIR:?REPORT_DIR required}"\nprintf ready\n' > report.sh
bash -n report.sh
if env -u REPORT_DIR bash report.sh > /dev/null 2>&1; then exit 1; fi
test "$(REPORT_DIR=reports bash report.sh)" = ready
printf 'missing environment rejected; explicit environment accepted\n'
