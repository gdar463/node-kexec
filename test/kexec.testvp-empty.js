import { exec } from 'child_process'
import { join, resolve } from 'path'
import assert from 'assert'

describe('kexec', function() {
  describe('#kexec() - echovp-empty', function() {
    it('should not print anything', function() {
      const echoFile = join(resolve("test"), './files/echovp-empty.sh')
      exec(echoFile, function(error, stdout, stderr) {
        assert(stdout.trim() === '')
        assert(stderr.trim() === '')
        assert(error === null)
      })
    })
  })
})
