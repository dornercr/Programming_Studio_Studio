# Worked C++ inside the Design Patterns Outline

Apply this small follow-up after `Design_Patterns_Study_Expansion.patch`.

## Where to find it

Open **Design Patterns → Outline → a chapter map**. After the numbered route through the chapter, the new **Eight examples you can trace and run** section shows code, expected output and explanation directly on the page. The chapter's **Execution trace** lesson also contains these examples.

There are eight examples for each of the 22 patterns and for the capstone: 184 in total. These bring the existing verified trace cases into the Outline; they do not claim to add another 184 coding challenges. Each example uses the completed chapter extension, clearly identified next to the code. The original chapter workshop remains separately labeled with its own original behavior.

- Visible call-site C++ and exact expected output.
- An explanation of the observed behavior beside the code.
- An expandable explanation of unfamiliar lambda, output, reference and loop syntax.
- Complete compilable source, a C++20 build command and a .cpp download under each example.
- A link to the related existing graded repair exercise.
- Prose-only mechanism/role lessons now show a relevant original implementation excerpt, source line references, line-by-line explanations, invariant and common mistake.
- Failure-cases lessons show the existing defective code excerpts and repair explanations directly.

No original topic, code resource, card, scenario, note or progress identity changes. The renderer derives the examples from the chapter data already loaded. It does not fetch the full Coding Lab catalog or other chapters to render the Outline. Other courses keep their existing reading views. The same teaching renderer is used by the web and optional offline builds.

## Install

From the project root:

```sh
git apply --check /absolute/path/Design_Patterns_Outline_Examples.patch
git apply /absolute/path/Design_Patterns_Outline_Examples.patch
npm run build
npm test
```

Or copy the ZIP's `changed-code/` contents into the project root, preserving paths, then build and test. Choose one installation method. Rebuild and publish the new `dist/` using the existing deployment workflow.

## Verification

```sh
node scripts/test-outline-examples.mjs
CHROMIUM_PATH=/absolute/path/to/chromium node scripts/outline-examples-browser.mjs
```

The first script uses g++ with C++20 and pthreads to compile and run all 184 actual downloadable programs. Every output must match the output displayed in the Outline, including whitespace. The formatter preserves quoted text and the parentheses within for-loop headers.

The browser script checks all 23 chapters beneath the GitHub Pages project path, visible code/output, complete-program download, mechanism explanations, defect explanations, related-lab navigation and mobile overflow. It confirms that Outline navigation does not fetch Coding Lab data. No remote compiler submissions are needed.

Verification records and visual captures are included in the delivery ZIP. This patch changes presentation and makes existing verified examples accessible in context; it does not replace educational prose with generated summaries or duplicate full workshops in every lesson.
