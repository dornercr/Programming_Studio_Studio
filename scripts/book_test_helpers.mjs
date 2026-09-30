export async function chooseBook(page,id){
  if(id==='my-material'){await page.locator('[data-action="personal-material"]').click();return;}
  await page.locator(`[data-course="${id}"].book-chapter-dropdown`).selectOption('resume');
  await page.waitForFunction(id=>document.querySelector('#book-dropdowns')?.dataset.currentCourse===id&&document.querySelector('#app')?.getAttribute('aria-busy')==='false',id);
}
export async function currentBook(page){return page.locator('#book-dropdowns').getAttribute('data-current-course');}
