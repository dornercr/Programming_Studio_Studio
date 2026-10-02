// Historical hash checks exclude exactly this separately delivered new course.
// The full source-to-build comparison still includes every Linux field.
export function withoutLinuxAdministration(data){return {...data,courses:data.courses.filter(c=>c.id!=='linux-system-administration')};}
