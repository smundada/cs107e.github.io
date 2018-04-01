---
layout: page
title: Guide to automated checker
---

You can learn more about automated testing, and check on its results
before you make a pull request, by logging into
[CircleCI](https://circleci.com) with your GitHub account. Then go to
[Add projects](https://circleci.com/add-projects) and then choose the
cs107e GitHub account, then click 'Watch project' next to your
assignments repo. Then click the newly appeared eye icon to view your
repo's test page on CircleCI, which should show an entry for each time
you've pushed so far.

<img title="Watching the CircleCI project."
src="../images/ci-watch-project.png" width="500">

<img title="CI page for assignments repo." src="../images/ci-repo.png" width="500">

Then you can also click the Details link in the test result in your
pull request (which you might want to make early -- you can always
push more commits to it) to see the CircleCI results page from testing
your latest solution.

<img title="Pull request page." src="/assignments/assign0/images/07-pull-request.png" width="500">

The results page shows the result of testing one particular solution
you pushed. You might need to scroll down to 'tests/run.sh' and dig
around to find the output if something like `make` failed.

<img title="make failure output." src="../images/ci-output.png"
width="600">

You can even download the larson.bin that was generated on the test
machine, although hopefully it's exactly the same as the one you're
building on your computer!

<img title="Build artifacts." src="../images/ci-artifacts.png" width="500">
