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

#ifndef CSS_CODECOMPLETIONMODEL_H
#define CSS_CODECOMPLETIONMODEL_H

#include <KTextEditor/CodeCompletionModel>
#include <KTextEditor/CodeCompletionModelControllerInterface>

namespace Css {

class CodeCompletionModel : public KTextEditor::CodeCompletionModel, public KTextEditor::CodeCompletionModelControllerInterface
{
    Q_OBJECT
    Q_INTERFACES(KTextEditor::CodeCompletionModelControllerInterface)

public:
    CodeCompletionModel(QObject *parent);
    void completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType) override;

    QVariant data(const QModelIndex & index, int role) const override;

    KTextEditor::Range completionRange(KTextEditor::View* view, const KTextEditor::Cursor &position) override;
    bool shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::Range& range, const QString &currentCompletion) override;
    bool shouldStartCompletion(KTextEditor::View* view, const QString& insertedText, bool userInsertion, const KTextEditor::Cursor& position) override;

    virtual void executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const;

    enum CompletionContext {
        NoContext,
        SelectorContext,
        PropertyContext,
        ValueContext
    };
private:
    CompletionContext m_completionContext;
    QList<QString> m_items;
};

}

#endif // CSS_CODECOMPLETIONMODEL_H
