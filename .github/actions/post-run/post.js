const { spawn } = require("child_process");

function run(cmd) {
  const subprocess = spawn(cmd, { stdio: "inherit", shell: true });
  subprocess.on("exit", (exitCode) => {
    process.exitCode = exitCode;
  });
}

run(process.env.INPUT_RUN);
