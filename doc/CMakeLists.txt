project(CppConcurrency-DOC NONE)

cmake_minimum_required(VERSION 3.0)

function(md2html MD)
	string(REPLACE ".md" ".html" HTML ${MD})
	set(MD_SRC "${CMAKE_SOURCE_DIR}/${MD}")
	add_custom_command(
		OUTPUT ${HTML}
		DEPENDS ${MD_SRC}
		COMMAND pandoc --from markdown_github-hard_line_breaks --to html --standalone -o ${HTML} ${MD_SRC}
		VERBATIM
	)
	add_custom_target(
		"${HTML}.stamp" ALL
		DEPENDS ${HTML}
	)
endfunction(md2html)

md2html("00-TipsReferences.md")
md2html("01-BasicThreadManagement.md")
md2html("02-MutexAndAtomics.md")
md2html("03-AsyncFuture.md")
md2html("04-ThreadedBuildingBlocksLoops.md")
md2html("05-TBBMutexAndAtomics.md")
md2html("06-TBBContainers.md")
md2html("07-TBBPipelines.md")
md2html("08-TBBGraphs.md")
md2html("09-TBBTasks.md")
md2html("XX-Answers.md")
md2html("XY-TBBGraphsExercise.md")
