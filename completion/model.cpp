/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "model.h"

namespace Css {

// extern int debugArea();

CodeCompletionModel::CodeCompletionModel(QObject *parent)
    : CodeCompletionModel2(parent)
{
}

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
    kDebug(/*debugArea()*/) << range;
    Q_UNUSED(view);
    Q_UNUSED(range);
    Q_UNUSED(invocationType);
    setRowCount(5);
    reset();
}

QVariant CodeCompletionModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == CodeCompletionModel::Name) {
        return QString::number(index.row());
    }
    return QVariant();
}


}
