import { exec } from 'child_process'
import { join, resolve } from 'path'
import assert from 'assert'

describe('kexec', function() {
  describe('#kexec() - echo', function() {
    it("should only print 'hello world'", function() {
      const echoFile = join(resolve("test"), './files/echo.sh')
      exec(echoFile, function(error, stdout, stderr) {
        assert.strictEqual(stdout.trim(), 'hello world')
        assert.strictEqual(stderr.trim(), '')
        assert.strictEqual(error, null)
      })
    })
  })
})
