#pragma once
#include "error.h"
#include "ownership.h"
#include "parser.h"

struct format_visit_ctx {
  view struct ast ast;
  int identation;
};

void format_visit(struct format_visit_ctx *ctx);
