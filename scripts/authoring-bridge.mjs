import fs from 'node:fs/promises';import {spawnSync} from 'node:child_process';
const commands=process.argv.includes('--coding')?['python3 scripts/make_coding_questions.py']:['python3 scripts/make_content.py','node scripts/render_diagrams.mjs','node scripts/render_systems_diagrams.mjs','node scripts/render_cpp_series_diagrams.mjs','python3 scripts/make_coding_questions.py'];
await import('./materialize-legacy.mjs');
for(const command of commands){const result=spawnSync(command,{shell:true,stdio:'inherit'});if(result.status!==0)throw Error('Authoring failed; intermediate source retained: '+command);}
// Keep the historical migration baseline immutable during later authoring.
const baseline=await fs.readFile('docs/migration/baseline.json');
try{const result=spawnSync(process.execPath,['scripts/migrate-content.mjs'],{stdio:'inherit'});if(result.status!==0)throw Error('Could not commit authored content to chapter files');}finally{await fs.writeFile('docs/migration/baseline.json',baseline);}
await fs.rm('src/content.json');await fs.rm('src/coding-content.json');
