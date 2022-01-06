/**
 * @file scp_trace.h
 *
 * @brief Componenet for drawing tracepoints.
 *
 * @ingroup SimpleCustomPlotInternal
 *
 * @author Frans Rosencrantz
 * Contact: Frans.Rosencrantz@gmail.com
 *
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <scp_datamodels.h>
#include <optional>

namespace scp {

/** @brief The label corner that is located at the tracepoint center.  */
enum class TraceLabelCornerPosition {
  top_left,
  top_right,
  bottom_left,
  bottom_right
};

/** @brief A struct that defines a tracepoint. */
template <class ValueType>
struct TracePoint : public juce::Component {
#if ((defined(_MSVC_LANG) && _MSVC_LANG > 201703L) || __cplusplus > 201703L)
  /** Spaceship */
  constexpr bool operator<=>(const TracePoint<ValueType>& rhs) {
    return this->getPosition() <=> rhs.getPosition();
  }
#else
  /** No spaceship  :( */
  constexpr bool operator==(const TracePoint<ValueType>& rhs) {
    return this->getPosition() == rhs.getPosition();
  }
#endif

  /** Compare with juce point. */
  constexpr bool operator==(const juce::Point<ValueType>& other_graph_values) {
    return m_graph_values == other_graph_values;
  }

  /** Set the graph value. */
  void setGraphValue(const juce::Point<ValueType>& graph_value);

  /** @internal */
  void resized() override;
  /** @internal */
  void paint(juce::Graphics& g) override;
  /** @internal */
  void lookAndFeelChanged() override;

  /** @internal */
  juce::LookAndFeel* m_lookandfeel;

  /** The x and y values of the tracepoint. */
  juce::Point<ValueType> m_graph_values;
};

/** @brief A struct that defines a tracepoint using floats. */
typedef TracePoint<float> TracePoint_f;

/** @brief A struct that defines a trace label. */
template <class ValueType>
struct TraceLabel : public juce::Component {
  /** Set the graph labels from point. */
  void setGraphLabelFrom(const juce::Point<ValueType>& graph_value);

  /** @internal */
  void resized() override;
  /** @internal */
  void paint(juce::Graphics& g) override;
  /** @internal */
  void lookAndFeelChanged() override;

  /** The x and y labels. */
  scp::Label m_x_label, m_y_label;

  /** @internal */
  juce::LookAndFeel* m_lookandfeel;
};

/** @brief A typedef defines a tracelabel using floats. */
typedef TraceLabel<float> TraceLabel_f;

/** @breif A struct that defines a tracelabel and a tracepoint */
template <class ValueType>
struct TraceLabelPoint {
  std::unique_ptr<TraceLabel<ValueType>> trace_label;
  std::unique_ptr<TracePoint<ValueType>> trace_point;
  const GraphLine* associated_graph_line{nullptr};
  TraceLabelCornerPosition trace_label_corner_pos{
      TraceLabelCornerPosition::top_left};
};

/** @breif A struct that defines a tracelabel and a tracepoint using floats. */
typedef TraceLabelPoint<float> TraceLabelPoint_f;

/**
 * \class Trace
 * \brief A class for drawing tracepoints
 *
 * The idea is to use this class to display the x, y value of a one more
 * points on a graph.
 */
class Trace {
 public:
  ~Trace();

  /*@breif Get the associated GraphLine.
   *
   * @param TracePoint juce::Componenet* a TracePoint component.
   * @return GraphLine* the associated GraphLine if found else nullptr
   */
  const GraphLine* getAssociatedGraphLine(const juce::Component* trace_point) const;


  /** @brief Add or remove a tracepoint.
   *
   * The tracepoint is removed if it's already exists.
   *
   * @param trace_point_coordinate the coordinate where the point will be drawn.
   * @param graph_line pointer to a graphline.
   * @return void.
   */
  void addOrRemoveTracePoint(const juce::Point<float>& trace_point_coordinate,
                             const GraphLine* graph_line);

  /** @brief Update the tracepoint bounds from graph attributes.
   *
   * @param graph_attributes common graph attributes.
   * @return void.
   */
  void updateTracePointBoundsFrom(const GraphAttributesView& graph_attributes);

  /** @brief Add the tracepoints to a parent component
   *
   * @param parent_comp the component that the tracepoint will be added to as
   * a child component.
   * @return void.
   */
  void addAndMakeVisibleTo(juce::Component* parent_comp);

  /** @brief Set the lookandfeel for the tracepoints and tracelabels.
   *
   * @param lnf pointer to the lookandfeel instance.
   * @return void.
   */
  void setLookAndFeel(juce::LookAndFeel* lnf);

  /** @breif Set the position of a single tracepoint.
   *
   * @param graph_attributes common graph attributes.
   * @param trace_point the tracepoint that will have the new position.
   * @param new_position the new position for the tracepoint.
   * @return void.
   */
  void setGraphPositionFor(juce::Component* trace_point,
                           const juce::Point<float>& new_position,
                           const GraphAttributesView& graph_attributes);

  /** @breif Set the coner position of a single tracelabel.
   *
   * @param trace_point the associated tracepoint.
   * @param mouse_position the position of the mouse.
   * @return void.
   */
  void setCornerPositionForLabelAssociatedWith(juce::Component* trace_point,
                            const juce::Point<int>& mouse_position);

  /** @brief Check if a juce::Component* is one of the added tracepoints.
   *
   * @param component a juce component
   * @return bool true if the component is one of the tracepoints.
   */
  bool isComponentTracePoint(const juce::Component* component) const;

  /** @brief Check if a juce::Component* is one of the added tracelabels.
   *
   * @param component a juce component
   * @return bool true if the component is one of the tracelabels.
   */
  bool isComponentTraceLabel(const juce::Component* component) const;

 private:
  /** @internal */
  void addSingleTracePointAndLabel(
      const juce::Point<float>& trace_point_coordinate,
      const GraphLine* graph_line);
  /** @internal */
  void removeSingleTracePointAndLabel(
      const juce::Point<float>& trace_point_coordinate);
  /** @internal */
  void updateSingleTraceLabelTextsAndBounds(
      TraceLabelPoint_f* trace_point_label,
      const GraphAttributesView& graph_attributes);
  /** @internal */
  void updateTracePointsLookAndFeel();
  /** @internal */
  std::vector<TraceLabelPoint_f>::const_iterator
  findTraceLabelPointIteratorFrom(const juce::Component* trace_point) const;
  /** @internal */
  juce::LookAndFeel* m_lookandfeel;
  /** @internal */
  std::vector<TraceLabelPoint_f> m_trace_labelpoints;
};

}  // namespace scp