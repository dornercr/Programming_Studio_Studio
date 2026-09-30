// Compatibility bridge for original authoring generators; generated files are ignored by Git.
import fs from 'node:fs/promises';import {assemble,codingSource} from './content-store.mjs';
await fs.writeFile('src/content.json',JSON.stringify(await assemble()));await fs.writeFile('src/coding-content.json',JSON.stringify(await codingSource()));
