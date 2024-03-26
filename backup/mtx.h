#pragma once

int mtx_create();

void mtx_lock(int mtx);

void mtx_unlock(int mtx);

void destroy_mtxs();
