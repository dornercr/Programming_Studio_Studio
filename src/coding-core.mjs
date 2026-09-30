// Pure compiler request / teaching-test helpers, shared by the app and its tests.
export const LAB_COMPILER = 'g142';
export const LAB_ENDPOINT = 'https://godbolt.org/api/compiler/g142/compile';
const cppString = value => JSON.stringify(String(value)).replace(/\\u([0-9a-f]{4})/gi,'\\u$1');
export function labBuildProgram(code, question, mode='run', nonce='local') {
  const source = '#line 1 "main.cpp"\n' + code + '\n';
  if (!question) return source;
  if (mode === 'run') return source + '#line 1 "study_driver.cpp"\n' + question.driver + '\n';
  if (!/^[a-zA-Z0-9_-]+$/.test(nonce)) throw new Error('Invalid test run identifier');
  const checks = question.tests.map(t=>`  check(${cppString(t.id)}, ${t.expectedExpression ? `study_lab::repr(${t.expectedExpression})` : cppString(t.expected)}, []{ return ${t.expression}; });`).join('\n');
  return source + `#line 1 "study_tests.cpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <exception>
namespace study_lab {
  template<class T> std::string repr(const T& v);
  template<class T> std::string repr(const std::vector<T>& v);
  template<class A, class B> std::string repr(const std::pair<A,B>& v);
  template<class T> std::string repr(const T& v) { std::ostringstream out; out << std::boolalpha << v; return out.str(); }
  template<class A, class B> std::string repr(const std::pair<A,B>& v) { return "("+repr(v.first)+", "+repr(v.second)+")"; }
  template<class T> std::string repr(const std::vector<T>& v) { std::string out="["; for(std::size_t i=0;i<v.size();++i) { if(i)out+=", "; out+=repr(v[i]); } return out+"]"; }
  std::string hex(const std::string& s) { static const char* digits="0123456789abcdef"; std::string out; for(unsigned char c:s){out+=digits[c>>4];out+=digits[c&15];} return out; }
}
int main() {
  int failures=0;
  auto check=[&](const char* id, const std::string& expected, auto action) {
    std::string actual; bool ok=false;
    try { actual=study_lab::repr(action()); ok=actual==expected; }
    catch(const std::exception& e){ actual=std::string("exception: ")+e.what(); }
    catch(...){ actual="exception: unknown"; }
    if(!ok)++failures;
    std::cout << "\\n@STUDY:${nonce}|" << id << "|" << (ok?"PASS":"FAIL") << "|" << study_lab::hex(actual) << "|" << study_lab::hex(expected) << "\\n";
  };
${checks}
  return failures ? 1 : 0;
}
`;
}
export function labRequest(source, stdin='') {
  return {source,lang:'c++',allowStoreCodeDebug:false,options:{userArguments:'-std=c++20 -Wall -Wextra -pedantic -pthread -fdiagnostics-color=never',compilerOptions:{executorRequest:true},filters:{execute:true},executeParameters:{args:[],stdin},tools:[],libraries:[]}};
}
export function labText(lines) {
  return (Array.isArray(lines)?lines.map(x=>typeof x==='string'?x:String(x?.text??'')).join('\n'):String(lines??'')).replace(/\x1b\[[0-9;]*[A-Za-z]/g,'').slice(0,160000);
}
function unhex(s) {
  if(s.length%2||!/^[0-9a-f]*$/i.test(s))throw new Error('Malformed test record');
  return new TextDecoder().decode(Uint8Array.from(s.match(/../g)||[],x=>parseInt(x,16)));
}
export function labParseResponse(payload, question=null, nonce='local') {
  if(!payload || typeof payload!=='object' || !('code' in payload))throw new Error('The compiler service returned an unexpected response. Your code is still saved.');
  const build=payload.buildResult||payload, run=payload.execResult||payload;
  const compiled=build.code===0;
  const diagnostics=labText(build.stderr)+'\n'+labText(build.stdout);
  let stdout=compiled&&run.didExecute?labText(run.stdout):'', stderr=compiled&&run.didExecute?labText(run.stderr):'';
  const records=new Map(), prefix=`@STUDY:${nonce}|`, remainder=[];
  for(const line of stdout.split('\n')){
    if(question&&line.startsWith(prefix)){
      const [id,status,actual,expected,...extra]=line.slice(prefix.length).split('|');
      if(!extra.length && ['PASS','FAIL'].includes(status) && question.tests.some(t=>t.id===id) && !records.has(id)){
        try { const a=unhex(actual),e=unhex(expected);records.set(id,{actual:a,expected:e,passed:status==='PASS'&&a===e});continue; } catch{}
      }
    }
    remainder.push(line);
  }
  const cases=question?question.tests.map(t=>({...t,...records.get(t.id),passed:records.get(t.id)?.passed===true,missing:!records.has(t.id)})):[];
  const completed=compiled && !!run.didExecute && !run.timedOut && !run.truncated;
  return {compiled,executed:!!run.didExecute,code:run.code,stdout:remainder.join('\n').replace(/\n+$/,''),stderr,diagnostics:diagnostics.trim(),timedOut:!!run.timedOut,truncated:!!run.truncated,cases,passed:!!question&&completed&&run.code===0&&cases.length>0&&cases.every(c=>c.passed)};
}
export function labDiagnosticHelp(diagnostics) {
  if(/expected ['‘];['’]/.test(diagnostics))return 'A statement may be missing its ending semicolon (;). Check the reported line and the line just before it.';
  if(/not declared in this scope|undeclared identifier/.test(diagnostics))return 'The compiler cannot find this name. Check spelling, where the name is declared, and whether its header is included.';
  if(/redefinition of.*main|multiple definition.*main/.test(diagnostics))return 'A question supplies main() for you. Keep only the requested function or class in the editor, or use the Playground for a complete program.';
  if(/no matching function|cannot convert|invalid conversion/.test(diagnostics))return 'The value types do not match this operation. Compare the parameter types, return type, and the values passed at the reported line.';
  if(/undefined reference/.test(diagnostics))return 'A named function was declared but no matching definition was linked. Check the required signature and whether every called function has a body.';
  if(/expected.*[}]/.test(diagnostics))return 'A brace may be missing or unmatched. Check where each function, loop, and class begins and ends.';
  if(/fatal error:.*No such file/.test(diagnostics))return 'This header is unavailable in the online environment. These labs use the standard library; local libraries and CUDA need their own build setup.';
  return 'Start with the first error. Check its file and line, then compare your function signature with the question. Later errors may be caused by the first one.';
}

// Check a whole book program against its recorded input, output, and exit status.
// A program can have main(), so it cannot be joined with the function-test main().
export function labCheckWorkedResult(result, check) {
  // The service returns lines, not an exact final-newline byte count.
  // Keep trailing spaces within a line: several textbook examples print them.
  const clean = value => String(value ?? '').replace(/\r\n/g,'\n').replace(/\n+$/,'');
  const expected = `Exit: ${check.exitCode}\nstdout:\n${clean(check.stdout)||'(empty)'}\nstderr:\n${clean(check.stderr)||'(empty)'}`;
  const actual = result.compiled && result.executed
    ? `Exit: ${result.code}\nstdout:\n${clean(result.stdout)||(result.stdout===''?'(empty)':result.stdout)}\nstderr:\n${clean(result.stderr)||'(empty)'}`
    : 'No completed result';
  const passed = result.compiled && result.executed && !result.timedOut && !result.truncated &&
    result.code===check.exitCode && clean(result.stdout)===clean(check.stdout) && clean(result.stderr)===clean(check.stderr);
  return {id:check.id,label:check.label,expression:`stdin: ${check.input||'(empty)'}`,
    expected,actual,hint:check.hint,passed,missing:!result.compiled||!result.executed};
}
