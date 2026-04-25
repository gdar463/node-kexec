import kexec from '../'

console.log(process.pid + ' - PID before exec')

kexec('sh', ['-c', 'echo "$$ - PID after exec"'])
