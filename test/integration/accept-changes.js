const fs = require("fs");
const prettier = require("prettier");

function copyHashes(failHashFile, refHashFile) {
  const failHashData = JSON.parse(fs.readFileSync(failHashFile));
  const refHashData = JSON.parse(fs.readFileSync(refHashFile));

  for (const testFilename in failHashData.test) {
    if (!refHashData.test[testFilename]) {
      refHashData.test[testFilename] = {
        refs: [failHashData.test[testFilename].refs[0]],
      };
    } else if (refHashData.test[testFilename].refs[0] !== "")
      refHashData.test[testFilename].refs[0] =
        failHashData.test[testFilename].refs[0];
  }

  let formattedRefHashDataReady = prettier.format(
    JSON.stringify(refHashData, null, 2),
    {
      parser: "json",
      tabWidth: 2,
    },
  );
  formattedRefHashDataReady.then((formattedRefHashData) => {
    fs.writeFileSync(refHashFile, formattedRefHashData);
  });
}

copyHashes(
  "test_report/results/test_cases/test_cases.json",
  "test_cases/test_cases.json"
);
/*
copyHashes(
	"test_report/results/tests/style/style_tests.json",
	"tests/style_tests.json"
)*/
